const {httpError}=require('../../helpers/handleError')
const sensoresmodelo=require('../models/sensores_vista')
// importar getIo lo estoy llamando
const { getIo } = require('../../src/socket'); 
// importar el módulo pathpara trabajar con rutas de archivos y directorios
const path = require('path'); 

const getItems= async(req,res)=>{
    // try{
    //     //datos que estan en el modelo
    //     const listarsensores=await sensoresmodelo.find({})
    //     res.send({data:listarsensores})
    // }catch(e){
    //     httpError(res,e)
    // }
    //ubicacion del index.html para mostrarlo en el link : https://d3nm9bfn-3300.use2.devtunnels.ms/api/1.0/sensores_vista
    res.sendFile(path.join(__dirname, '../public/index.html')); 
}

const getItem=(req,res)=>
{
}

const createItem= async(req,res)=>{
    // try{
        // //datos que estan en el modelo
        // const {sensorPH,sensorTurbidez,sensorOxygenConcentration,sensorOxygenSaturacion,sensorConductividad}=req.body
        // const deallesrespuesta=await usuariomodelo.create({
        //     sensorPH,sensorTurbidez,sensorOxygenConcentration,sensorOxygenSaturacion,sensorConductividad
        // })
        // res.send({data:deallesrespuesta})
        try {
            const {
                sensorPH,
                sensorTurbidez,
                sensorOxygenConcentration,
                sensorOxygenSaturacion,
                sensorConductividad
            } = req.body;
    
            const detallesrespuesta = await sensoresmodelo.create({
                sensorPH,
                sensorTurbidez,
                sensorOxygenConcentration,
                sensorOxygenSaturacion,
                sensorConductividad
            });
            // Esto es para obtner la instancia de Socket.IO
            const io = getIo();
            // Eso es para emitir datos
            io.emit('newSensorData', detallesrespuesta); 
            res.send({ data: detallesrespuesta });
    }catch(e){
        httpError(res,e)
    }
}
const updateItem=async (req,res)=>
{
    try {
        const { id } = req.params;
        const {
            sensorPH,
            sensorTurbidez,
            sensorOxygenConcentration,
            sensorOxygenSaturacion,
            sensorConductividad
        } = req.body;

        // actualizar el registro en la base de datos
        const sensorActualizado = await sensoresmodelo.findByIdAndUpdate(id, {
            sensorPH,
            sensorTurbidez,
            sensorOxygenConcentration,
            sensorOxygenSaturacion,
            sensorConductividad
        }, { new: true });

        if (!sensorActualizado) {
            return res.status(404).send({ error: "Sensor no encontrado" });
        }

        // emitir los cambios de los sensores actualizados a todos los clientes conectados
        const io = getIo();
        io.emit('updateSensorData', sensorActualizado);

        res.send({ data: sensorActualizado });
    } catch (e) {
        httpError(res, e);
    }
};

const deleteItem = (req, res) => {
}

module.exports={getItems,getItem,createItem,updateItem,deleteItem}