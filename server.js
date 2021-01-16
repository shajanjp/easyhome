const express = require("express");
const ws = require('ws');
const http = require("http");

const app = express(); 
const server = http.createServer(app);

const wss = new ws.Server({ server, path: '/ws' });

app.get('/broadcast', (req, res) => {
  broadcast("VISIT");
  res.json({ "message": "done" });
})

wss.on('connection', (socket) => {
  socket.on('message', (message) => {
    console.log('received: %s', message);
  });
});

function broadcast(message) {
  wss.clients.forEach((client) => {
    if (client !== ws && client.readyState === ws.OPEN) {
      client.send(message);
    }
  });
}

server.listen(process.env.PORT, () => {
  console.log("web server started");
})