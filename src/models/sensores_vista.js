const mongoose = require("mongoose");
const sensores_vista = new mongoose.Schema(
    //esturctura para recibir los datos
    {
        sensorPH: {
            type: String,
        },
        sensorTurbidez: {
            type: String,
        },
        sensorOxygenConcentration: {
            type: String,
        },
        sensorOxygenSaturacion: {
            type: String,
        },
        sensorConductividad:{
            type:String
        },
},
    {
        //habilitar automáticamente la creación y actualización de campos de tiempo
    timestamps: true,
        //se utiliza para deshabilitar el campo de versión automático que Mongoose añade a los documentos.
    versionKey: false,
    }
);

module.exports = mongoose.model("sensores_vista", sensores_vista);
