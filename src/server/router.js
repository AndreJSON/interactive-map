var express = require('express');
var router = express.Router();
var events = require('./events.json')

router.get('/', function (req, res) {
	res.json({msg: "Hello World!"});
	res.end();
});

router.get('/events', function (req, res) {
	res.json(events);
	res.end();
});

module.exports = router;