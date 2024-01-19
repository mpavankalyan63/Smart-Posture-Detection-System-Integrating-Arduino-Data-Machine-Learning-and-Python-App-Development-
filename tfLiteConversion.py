try:
    import tensorflow as tf
    converter = tf.lite.TFLiteConverter.from_saved_model(r'D:\masters\Embedded Machine learning\Project_4\Posture_Detection_Model')
    tflite_model = converter.convert()
    tflite_model_path = r"D:\masters\Embedded Machine learning\Project_4\Posture_Detection_LiteModel"
    tf.io.write_file(tflite_model_path, tflite_model)
    with open('Posture_Detection_Model.tflite', 'wb') as f:
        f.write(tflite_model)
    print("Module Converted and Saved")
except:
    print("Error Converting Model")