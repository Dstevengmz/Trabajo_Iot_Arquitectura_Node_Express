
//inicializar socket parate del cliente
const socket = io({
    // Forzar uso de WebSockets
    //WebSocket permite abrir una sesión de 
    //comunicación interactiva bidireccional entre el navegador del usuario y un servidor
    transports: ['websocket'],
    // este parte debe permitir reconexión automática  
    reconnection: true,  
    // en caso de que falle hacer intentos ilimitados de reconexión
    reconnectionAttempts: Infinity,  
    // definir un intervalo de tiempo de espera entre intentos de reconexión
    reconnectionDelay: 1000,  
    // este codigo es para el tiempo de espera de conexión inicial
    timeout: 20000,  
    // este es para el tiempo de espera de ping para mantener la conexión activa
    pingTimeout: 60000  
});

// Confirmar conexión al servidor
socket.on('connect', () => {
    console.log('Conectado al servidor de Socket.io');
});

// esto debe permitir escuchar para  cuando se emitan nuevos datos de sensores sevisualizan el index.html
socket.on('newSensorData', (data) => {
    console.log('Recibido newSensorData:', data);
    document.getElementById('sensorPH').innerText = data.sensorPH;
    document.getElementById('sensorTurbidez').innerText = data.sensorTurbidez;
    document.getElementById('sensorOxygenConcentration').innerText = data.sensorOxygenConcentration;
    document.getElementById('sensorOxygenSaturacion').innerText = data.sensorOxygenSaturacion;
    document.getElementById('sensorConductividad').innerText = data.sensorConductividad;
});

//una vez aparezcan los datos se deben ir actualizando por los nuevos
// esto debe permitir escuchar cuando los datos de sensores se actualicen
socket.on('updateSensorData', (data) => {
    console.log('Recibido updateSensorData:', data);
    document.getElementById('sensorPH').innerText = data.sensorPH;
    document.getElementById('sensorTurbidez').innerText = data.sensorTurbidez;
    document.getElementById('sensorOxygenConcentration').innerText = data.sensorOxygenConcentration;
    document.getElementById('sensorOxygenSaturacion').innerText = data.sensorOxygenSaturacion;
    document.getElementById('sensorConductividad').innerText = data.sensorConductividad;
});

// codigo  para manejar la desconexión es decir que siga funcionando
socket.on('disconnect', (reason) => {
    console.log('Desconectado del servidor. Razón:', reason);
});

// capturar algun error que se pueda  presentar se podra visualizar
socket.on('error', (err) => {
    console.log('Error de conexión:', err);
});
