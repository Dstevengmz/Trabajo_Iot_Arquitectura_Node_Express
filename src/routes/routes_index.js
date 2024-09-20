const express = require('express');
const router = express.Router();
const fs = require('fs');
const pathRouter = `${__dirname}`;

const removerextension = (fileName) => {
    return fileName.split('.').shift();
};


fs.readdirSync(pathRouter).filter((file) => {
    const fileconextension = removerextension(file);
    const skip = ['routes_index'].includes(fileconextension);
    
    if (!skip) {
         router.use(`/${fileconextension}`, require(`./${fileconextension}`)); // Corrige aquí
        console.log('CARGAR RUTA---->', fileconextension);
    }
});
router.get('*',(req,res)=>{
    res.status(404)
    res.send({Error: 'no found'})
})

module.exports = router;
