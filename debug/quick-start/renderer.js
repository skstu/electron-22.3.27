/**
 * This file is loaded via the <script> tag in the index.html file and will
 * be executed in the renderer process for that window. No Node.js APIs are
 * available in this process because `nodeIntegration` is turned off and
 * `contextIsolation` is turned on. Use the contextBridge API in `preload.js`
 * to expose Node.js functionality from the main process.
 */


const ipcRenderer = require('electron').ipcRenderer;
/*
ipcRenderer.send('message-name', 'hello from renderer');

ipcRenderer.on('message-name', (event, arg) => {
  console.log(arg); 
});*/

window.addEventListener('DOMContentLoaded', () => {
  const replaceText = (selector, text) => {
    const element = document.getElementById(selector)
    if (element) element.innerText = text
  }

  for (const type of ['chrome', 'node', 'electron']) {
    replaceText(`${type}-version`, process.versions[type])
  }
/*
  ipcRenderer.on('genJson', (event,arg) => {
    console.log(arg);
  });

  ipcRenderer.on('test_btn', (event) => {
    console.log('Receved message on renderer.');
  });
*/
  document.getElementById("test_btn").addEventListener("click", () => {
    let req = `{"num":0,"str":"hello!"}`;
    ipcRenderer.send("genJson",req);
    ipcRenderer.once('genJson', (event, arg) => {
      console.log(arg);
    });
    
  });

})