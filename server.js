const express = require("express");
const ws = require('ws');
const http = require("http");

const app = express(); 
const server = http.createServer(app);

const wss = new ws.Server({ server, path: '/ws' });

app.get('/broadcast', (req, res) => {
  const message = req.query.message || '';
  broadcast(message);
  res.json({ success: "true", message });
})

wss.on('connection', (socket) => {
  socket.on('message', (message) => {
    console.log('received: ', message);
    broadcast(message);
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