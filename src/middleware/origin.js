const revisar=(req,res,next)=>{
    console.log(req.headers)
    next()
}
module.exports=revisar