
//Iniciarlizar el socket io de parte del cliente visualiar la conexion en la consola de vscode
const socketIo = require('socket.io');

let io;

function init(server) {
    io = socketIo(server, {
        // Este pedazo es para aumentar el tiempo de espera del ping a 60 segundos
        pingTimeout: 60000,  
        // Este pedazo es para el ntervalo para enviar pings cada 25 segundos
        pingInterval: 25000  
    });

    io.on('connection', (socket) => {
        console.log('Nuevo cliente conectado', socket.id);

        socket.on('disconnect', (reason) => {
            console.log('Cliente desconectado', socket.id, 'Razón:', reason);
        });
    });
}

function getIo() {
    if (!io) {
        throw new Error('Socket.io no está inicializado');
    }
    return io;
}
//exportarlos en todo el proyecto
module.exports = { init, getIo };
