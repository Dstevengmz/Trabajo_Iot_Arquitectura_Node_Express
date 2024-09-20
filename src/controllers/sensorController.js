const Sensores = require('../models/Sensores');

// Controlador para guardar los datos de los sensores
exports.saveSensoresData = async (req, res) => {
    try {
        const sensorData = req.body;  // Los datos enviados desde el Arduino en formato JSON
        const newSensorData = new Sensores(sensorData);  // Crea un nuevo documento usando el modelo
        await newSensorData.save();  // Guarda el documento en la base de datos
        res.status(201).json({ message: "Datos de sensores guardados correctamente" });
    } catch (error) {
        res.status(500).json({ message: "Error al guardar los datos", error });
    }
};
