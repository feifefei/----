Page({
  // 跳转到音乐播放页面
  jumpToMusic: function() {
    wx.navigateTo({
      url: '/pages/index/index'
    })
  },
  jumpToCaokong: function() {
    wx.navigateTo({
      url: '/pages/huoqu/huoqu'
    })
  },
  jumpTocaokong: function() {
    wx.navigateTo({
      url: '/pages/caokong/caokong'
    })
  }
})