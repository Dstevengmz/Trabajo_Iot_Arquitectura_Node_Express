const Sensores = require('../models/Sensores');
// Controlador para guardar los datos de los sensores
exports.saveSensoresData = async (req, res) => {
    try {
        // Los datos enviados desde el Arduino en formato JSON
        const sensorData = req.body;  
        // Crea un nuevo documento usando el modelo
        const newSensorData = new Sensores(sensorData);  
        // Guarda el documento en la base de datos
        await newSensorData.save();  
        res.status(201).json({ message: "Datos de sensores guardados correctamente" });
    } catch (error) {
        res.status(500).json({ message: "Error al guardar los datos", error });
    }
};
