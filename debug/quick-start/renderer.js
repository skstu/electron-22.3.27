/**
 * This file is loaded via the <script> tag in the index.html file and will
 * be executed in the renderer process for that window. No Node.js APIs are
 * available in this process because `nodeIntegration` is turned off and
 * `contextIsolation` is turned on. Use the contextBridge API in `preload.js`
 * to expose Node.js functionality from the main process.
 */

/*
const ipcRenderer = require('electron').ipcRenderer;

ipcRenderer.send('message-name', 'hello from renderer');

const ipcMain = require('electron').ipcMain;

ipcMain.on('message-name', (event, arg) => {
  console.log(arg); 
});
*/
