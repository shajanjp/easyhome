const WebSocket = require('ws');
const wss = new WebSocket.Server({ port: process.env.PORT });
wss.on('connection', (ws) => {
  ws.on('message', (message) => {
    console.log('received: %s', message);
  });
  setInterval(()=> {
    ws.send('VISIT');
  }, 30000);
});