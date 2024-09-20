const express = require('express');
const router = express.Router();
const revisar =require('../middleware/origin')
const {getItems,getItem,createItem,updateItem,deleteItem}=require('../controllers/usuarioController')

// Listar todos los usuarios
router.get('/', getItems);

// Obtener detalle de un usuario por ID
router.get('/:id', getItem); 

// Crear un nuevo usuario
router.post('/',revisar,createItem); 

// Actualizar un usuario
router.patch('/:id', updateItem); 

// Borrar un usuario
router.delete('/:id',deleteItem); 

// Exportar el router
module.exports = router;
