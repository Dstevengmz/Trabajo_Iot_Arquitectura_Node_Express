const mongoose = require("mongoose");
const sensores = new mongoose.Schema(
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
        sensorConductividad:{
            type:String
        },
},
    {
    timestamps: true,
    versionKey: false,
    }
);

module.exports = mongoose.model("sensores", sensores);
