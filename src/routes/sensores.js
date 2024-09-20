const express = require("express");
const router = express.Router();
const sensoresController = require("../controllers/sensorController");

// Ruta para recibir y guardar los datos de los sensores
router.post("/sensores", sensoresController.saveSensoresData);

module.exports = router;
