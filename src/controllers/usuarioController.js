const {httpError}=require('../../helpers/handleError')
const usuariomodelo=require('../models/usuario')

const getItems= async(req,res)=>{
    try{
        //datos que estan en el modelo
        const listarUsuarios=await usuariomodelo.find({

        })
        res.send({data:listarUsuarios})
    }catch{
        httpError(res,e)
    }
}

const getItem=(req,res)=>{
}

const createItem= async(req,res)=>{
    try{
        //datos que estan en el modelo
        const {nombre,edad,correo}=req.body
        const deallesrespuesta=await usuariomodelo.create({
            nombre,edad,correo
        })
        res.send({data:deallesrespuesta})
    }catch{
        httpError(res,e)
    }
}

const updateItem=(req,res)=>{
}

const deleteItem = (req, res) => {
}

module.exports={getItems,getItem,createItem,updateItem,deleteItem}