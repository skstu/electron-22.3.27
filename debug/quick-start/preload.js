/**
 * The preload script runs before `index.html` is loaded
 * in the renderer. It has access to web APIs as well as
 * Electron's renderer process modules and some polyfilled
 * Node.js functions.
 *
 * https://www.electronjs.org/docs/latest/tutorial/sandbox
 */

const { ipcRenderer } = require("electron");


//ipcRenderer.on('genJson', (event, arg) => {
  //console.log(arg);
//});
/*
window.addEventListener('DOMContentLoaded', () => {
  const replaceText = (selector, text) => {
    const element = document.getElementById(selector)
    if (element) element.innerText = text
  }

  for (const type of ['chrome', 'node', 'electron']) {
    replaceText(`${type}-version`, process.versions[type])
  }

  ipcRenderer.on('*', (event) => {
    console.log('Receved message on renderer.');
  });

  ipcRenderer.on('test_btn', (event) => {
    console.log('Receved message on renderer.');
  });

  document.getElementById("test_btn").addEventListener("click", () => {
    let req = `{"num":0,"str":"hello!"}`;
    ipcRenderer.send("genJson",req);
  });
*/
