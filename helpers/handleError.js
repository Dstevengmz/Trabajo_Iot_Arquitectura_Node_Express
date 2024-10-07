const httpError=(res,err)=>{
    console.log(err)
    res.status(500)
    res.send({errro:'Algo ocurrio'})
};
module.exports = { httpError };

//para los errores