var express = require('express');
var app = express();
var path = require('path');
var bodyParser = require('body-parser');
app.use(bodyParser.urlencoded({ extended: true })); // support encoded bodies

app.get('/', function (req, res) {
  res.sendFile(path.join(__dirname + '/index.html'));
});
app.post('/', function (req, res) {
  const { spawn } = require('child_process');
  const ls = spawn(req.body.comando, [req.body.argumentos]);
  ls.stdout.on('data', (data) => {
  res.send(String(data));
  });
});
app.listen(3000, function () {
  console.log('Example app listening on port 3000!');
});

