require('dotenv').config();
const express = require('express');
const cors = require('cors');
const mongoose = require("mongoose");
const { dbConnect } = require('./config/mongo');
const sensorRoutes = require('./src/routes/sensores');


const app = express();

const PORT = process.env.PORT || 3200;
app.use(cors());
//codigo parsear json 
app.use(express.json());


//Rutas
app.use('/api/1.0', require(`./src/routes/routes_index`));
app.use('/api/1.0', sensorRoutes); //esta era para ver si depronto se almacenaban pero se necesita un servidor en la nube tambien.

(async () => {
    await dbConnect(); // Conexion db se establezca
    app.listen(PORT, () => {
        console.log('El servidor está corriendo en el puerto:', PORT);
    });
})();
