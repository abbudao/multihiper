var http = require('http');
var fs = require('fs');
var querystring = require('querystring');

function processPost(req, response, callback) {
  var queryData = "";
  if(typeof callback !== 'function') return null;

  if(req.method == 'POST') {
    req.on('data', function(data) {
      queryData += data;
      if(queryData.length > 1e6) {
        queryData = "";
        response.writeHead(413, {'Content-Type': 'text/plain'}).end();
        req.connection.destroy();
      }
    });

    req.on('end', function() {
      req.post = querystring.parse(queryData);
      callback();
    });

  } else {
    response.writeHead(405, {'Content-Type': 'text/plain'});
    response.end();
  }
}
http.createServer(function (req, res) {
  if(req.method == 'POST') {
    processPost(req, res, function() {
      // Use req.post here
      const { spawn } = require('child_process');
      const ls = spawn(req.post["comando"], [req.post["argumentos"]]);
      ls.stdout.on('data', (data) => {
        console.log(`stdout: ${data}`);
      });
      res.writeHead(200, "OK", {'Content-Type': 'text/plain'});
      res.end();
    });
  } else {
    fs.readFile('1.html', function(err, data) {
      res.writeHead(200, {'Content-Type': 'text/html'});
      res.write(data);
      res.end();
    });    }
}).listen(8080);
