// pages/huoqu/huoqu.js
// demo1/demo1.js
const deviceid ='1099831083'//你的设备id 
const apikey = 'Ngie8by96a7PZhV6M8xbF3QK2zE='//你的apikey
Page({

  jumpToMusic: function() {
    wx.navigateTo({
      url: '/pages/main/main'
    })
  },
  /**
   * 页面的初始数据
   */
  data: {
    data_list: []

  },
  onLoad(options) {
  },
  onReady() {
  },
  onShow() {
  },
  onHide() {
  },
  onUnload() {
  },
  onPullDownRefresh() {
  },
  onReachBottom() {
  },
  onShareAppMessage() {
  },
  OneNet_Get: function(){
    let that=this
    wx.request({
      url: "https://api.heclouds.com/devices/" + deviceid + "/datastreams",
      method:'GET',
      header:{
        "content-type": 'application/x-www-form-urlencoded',
        "api-key": apikey
      },
      success(res){
        console.log(res.data.data[0])
        if (res.data.data) {
          that.setData({
            data_list: res.data.data
          })
        }
        if(res.statusCode === 200){
          that.setData({
            temp1: res.data.data[1].current_value,
            temp2: res.data.data[2].current_value,
          })
        }
      },
      fail: function(res){
        wx.showToast({ title: '系统错误' })
      },
      complete:function(res){
        wx.hideLoading()
      }
    })
  }
})