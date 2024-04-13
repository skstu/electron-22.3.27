const { app, BrowserWindow } = require('electron')

function createWindow () {
  // 创建主窗口
  const mainWindow = new BrowserWindow({
    width: 800,
    height: 600,
    // 添加以下代码以自动打开 DevTools
    webPreferences: {
      devTools: true,
    }
  })

  // 加载应用内容
  mainWindow.loadURL('index.html')
}

app.on('ready', createWindow)

app.on('window-all-closed', () => {
  if (process.platform !== 'darwin') {
    app.quit()
  }
})
