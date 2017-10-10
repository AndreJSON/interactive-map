'use strict';

var port = 8082;
var express = require('express');
var app = express();

var bodyParser = require('body-parser');
app.use(bodyParser.json());

var router = require('./router.js');
app.use('', router);

app.listen(port);
console.log("Server is now listening on port: " + port);