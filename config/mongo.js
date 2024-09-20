// Conexion a la base de datos
const mongoose = require('mongoose');
async function dbConnect() {
    const DB_URI = process.env.DB_URI;
    try {
        await mongoose.connect(DB_URI);
        console.log('Conexión Correcta a la base de datos');
    } catch (err) {
        console.error('Problemas en la conexión a la base de datos', err);
    }
}

module.exports = {
    dbConnect
};
