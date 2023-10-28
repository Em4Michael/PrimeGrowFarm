const SensorData = require('../models/SensorData');

const saveSensorData = async (sensorData) => {
  try {
    delete sensorData.timestamp;
    sensorData.timestamp = new Date();
    const newSensorData = new SensorData(sensorData);
    await newSensorData.save();
    return newSensorData;
  } catch (error) {
    throw error;
  }
};


module.exports = {
  saveSensorData,
};
