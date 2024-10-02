const {httpError}=require('../../helpers/handleError')
const sensoresmodelo=require('../models/sensores_vista')

const getItems= async(req,res)=>{
    try{
        //datos que estan en el modelo
        const listarsensores=await sensoresmodelo.find({

        })
        res.send({data:listarsensores})
    }catch{
        httpError(res,e)
    }
}

const getItem=(req,res)=>{
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
            res.send({ data: detallesrespuesta });
    }catch{
        httpError(res,e)
    }
}
const updateItem=(req,res)=>{
}

const deleteItem = (req, res) => {
}

module.exports={getItems,getItem,createItem,updateItem,deleteItem}