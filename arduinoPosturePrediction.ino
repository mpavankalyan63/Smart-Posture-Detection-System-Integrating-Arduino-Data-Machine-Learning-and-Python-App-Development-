#include <Arduino_LSM9DS1.h>

#include <TensorFlowLite.h>
#include <tensorflow/lite/micro/all_ops_resolver.h>
#include <tensorflow/lite/micro/micro_error_reporter.h>
#include <tensorflow/lite/micro/micro_interpreter.h>
#include <tensorflow/lite/schema/schema_generated.h>
#include <tensorflow/lite/version.h>

// Change path where model header file is stored
#include <C:/Users/shubh/EmbeddedML/Miniprojects/mp4/model.h>

// global variables used for TensorFlow Lite (Micro)
tflite::MicroErrorReporter tflErrorReporter;
tflite::AllOpsResolver tflOpsResolver;

const tflite::Model* tflModel = ::tflite::GetModel(model);
tflite::MicroInterpreter* tflInterpreter = nullptr;
TfLiteTensor* tflInputTensor = nullptr;
TfLiteTensor* tflOutputTensor = nullptr;

// Create a static memory buffer for TFLM, the size may need to
// be adjusted based on the model you are using
constexpr int tensorArenaSize = 50 * 1024;
byte tensorArena[tensorArenaSize] __attribute__((aligned(16)));

float imuData[25][3]; // Array for storing IMU data
int j;
int s = 0;
float h; 
float u = 0;
int ans;

void setup() {
  Serial.begin(115200);
  // while (!Serial);
  // Serial.println("Started");
  if (!IMU.begin()) {
    Serial.println("Fail to initialize IMU");
    while (1);
  }

  // Create an interpreter to run the model
  tflInterpreter = new tflite::MicroInterpreter(tflModel, tflOpsResolver, tensorArena, tensorArenaSize, &tflErrorReporter);

  // Allocate memory for the model's input and output tensors
  tflInterpreter->AllocateTensors();
  // Get pointers for the model's input and output tensors
  tflInputTensor = tflInterpreter->input(0);
  tflOutputTensor = tflInterpreter->output(0);
}


void loop() {
  while (true) {
    j = Serial.readString().toInt();
    if (j != 1 && j != 2 && j != 3) {
      if (j != 0){Serial.println("Error, give correct input");}
      continue;
    }
    sensorDataReader(j);

    // printImuData();

    for (int a=0; a<25; a++){
      for (int b=0; b<3; b++){
        tflInputTensor->data.f[a * 3 + b] = imuData[a][b];
      }
    }
    // Run inferencing
    TfLiteStatus invokeStatus = tflInterpreter->Invoke();
    if (invokeStatus != kTfLiteOk) {
      Serial.println("Invoke failed!");
      while (1);
      return;
    }

    // Loop through the output tensor values from the model
    // for (int i = 0; i < 5; i++) {
    //   Serial.print(tflOutputTensor->data.f[i]);
    //   if (i != 5){
    //     Serial.print(",");
    //   }
    // }
    // Serial.println();
    
    for (int i = 0; i < 5; i++) {
      h = tflOutputTensor->data.f[i];
      u = tflOutputTensor->data.f[s];
      if (h>=u){
        s = i;
      }
    }

    ans = s;
    s = 0;
    switch (ans){
      case 0:
        Serial.println("Supine");
        break;
      case 1:
        Serial.println("Prone");
        break;
      case 2:
        Serial.println("Side");
        break;
      case 3:
        Serial.println("Sitting");
        break;
      case 4:
        Serial.println("Unknown");
        break;
    }
  }
}

void sensorDataReader(int sensorNumber) {
  for (int i = 0; i<25; i++) {
    if (j == 1){
      if (IMU.accelerationAvailable()) {
        IMU.readAcceleration(imuData[i][0], imuData[i][1], imuData[i][2]);  
      }
    } else if (j == 2){
      if (IMU.gyroscopeAvailable()) {
        IMU.readGyroscope(imuData[i][0], imuData[i][1], imuData[i][2]);       
      }
    } else if (j == 3){
      if (IMU.magneticFieldAvailable()) {
        IMU.readMagneticField(imuData[i][0], imuData[i][1], imuData[i][2]);   
      }
    }
    delay(80);
  }
}

// void printImuData(){
//   for (int a = 0; a<25; a++){
//       Serial.print(imuData[a][0]);
//       Serial.print(",");
//       Serial.print(imuData[a][1]);
//       Serial.print(",");
//       Serial.println(imuData[a][2]);
//     }
// }

