//LIBRERIAS DESDE LA VERSION 20 DE NODE SE DEBE USAR EJEMPLO require('node:express) pero a mi me sabe salir errores
require('dotenv').config();
const express = require('express');
const cors = require('cors');
const mongoose = require("mongoose");
const { dbConnect } = require('./config/mongo');
const sensorRoutes = require('./src/routes/sensores');
const http = require('http');
const { init } = require('./src/socket');
//inicializar express y creamos el server http
const app = express();
const server = http.createServer(app);

//codigo para llamar a la carpeta public es decir al index.html
app.use(express.static(__dirname + '/src/public'));

const PORT = process.env.PORT || 3200;
// app.use(cors(
    
// ));

 // Encabezados permitidos, Métodos permitidos, Permitir acceso desde cualquier origen
app.use(cors({
    origin: '*',  
    methods: ['GET', 'POST', 'PATCH', 'DELETE'],  
    allowedHeaders: ['Content-Type', 'Authorization'] 
}));
//codigo parsear json 
app.use(express.json());


//Rutas solo esta funcionando esta : app.use('/api/1.0', require(`./src/routes/routes_index`));
app.use('/api/1.0', require(`./src/routes/routes_index`));
//esta era para ver si depronto se almacenaban pero se necesita un servidor en la nube tambien.
app.use('/api/1.0', sensorRoutes); 

//este pedazo de codigo es para inicializar el socke.io
init(server);

(async () => {
    // Conexion db
    await dbConnect(); 
    // app.listen(PORT, () => {
    server.listen(PORT, () => {
        console.log('El servidor está corriendo en el puerto:', PORT);
    });
})();
