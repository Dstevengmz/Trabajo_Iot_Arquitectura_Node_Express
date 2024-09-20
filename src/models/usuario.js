const mongoose=require('mongoose')

const usuarios=new mongoose.Schema({
    nombre:{
        type:String
    },
    edad:{
        type:Number
    },
    correo:{
        type:String
    },
    
},
{
    timestamps:true,
    versionKey:false
})

module.exports=mongoose.model('usuario',usuarios)
