
//Librerias express , router,midlewar todavia sin usar para authentiaciones, ruta de controladores
const express = require('express');
const router = express.Router();
const revisar =require('../middleware/origin')
const {getItems,getItem,createItem,updateItem,deleteItem}=require('../controllers/sensoreController_vista')

// Listar todos los usuarios
router.get('/', getItems);

// Obtener detalle de un usuario por ID
router.get('/:id', getItem); 

// Crear un nuevo usuario
router.post('/',createItem);


// Actualizar un usuario
router.patch('/:id', updateItem); 

// Borrar un usuario
router.delete('/:id',deleteItem); 

// Exportar el router
module.exports = router;
