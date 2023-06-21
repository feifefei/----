var setTimer;//把时间函数变量设为全局
Page({

  data: {
    start_flag: false,//倒计时开始或暂停的flag
    timestamp: 5400,//倒计时的总共的秒数
    time: '90:00',//从timestamp转换成的‘xx：xx’格式的时间，用来显示在wxml页面
    textValue:0
  },

  
  changeFlag: function () {//点击A按钮
    this.setData({
      start_flag: !this.data.start_flag//把flag置为相反
    })
    if (this.data.start_flag == true) this.timer();//如果flag为true 开始倒计时函数timer()
    else {//否则即为停止，清除全局变量的计时函数，实现时间的停止
      clearInterval(setTimer);
    }
  },
  time_change: function (timestamp) {//时间戳转化成‘xx:xx’的可读形式
    var timem = 0, times = 0;
    timem = parseInt(timestamp / 60);
    times = ((timestamp % 60 < 10) ? ('0' + timestamp % 60) : timestamp % 60);
    return (timem + ':' + times);
  },
  //倒计时函数
  timer: function () {
    let promise = new Promise((resolve, reject) => {//ES6的语法，用就行，不需要看懂
      setTimer = setInterval(//时间循环函数
        () => {
          this.setData({//每隔一秒，时间戳-1，对应转化一次timestamp
            timestamp: this.data.timestamp - 1,
            time: this.time_change(this.data.timestamp - 1)
          })
          if (this.data.timestamp <= 0) {//如果时间为0，重置数据
            wx.navigateTo({
              url: '/pages/main/main',
            })
            this.setData({
              timestamp: 5400,
              start_flag: false,
              time: '90:00'
            })
            resolve(setTimer)
          }
        }
        , 1000)
    })
    promise.then((setTimer) => {
      clearInterval(setTimer)
    })
  },

  clear: function () {//点击B按钮,重置并清除全局变量的计时函数
    this.setData({
      timestamp: 5400,
      start_flag: false,
      time: '90:00'
    })
    clearInterval(setTimer);
  },

  bindTextInput: function (event) {//绑定input框的数据
    this.setData({
      textValue: event.detail.value
    })
  },

  set: function () {//点击C按钮,重置倒计时时间数据
    this.setData({
      timestamp: this.data.textValue,
      start_flag: false,
      time: this.time_change(this.data.textValue)
    })
    clearInterval(setTimer);
  },
  jumpToMusic: function() {
    wx.navigateTo({
      url: '/pages/main/main'
    })
  },
})
