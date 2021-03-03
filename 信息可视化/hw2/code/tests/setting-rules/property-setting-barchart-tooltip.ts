// 这里记录的是图表各种默认配置，当用户在测试页面选择不同的选项时，也就是是选择了这里的某组配置参数。
export default {
    tooltipIsShow: {
      type: 'boolean',
      title: '是否显示提示框',
      default: true
    },
    tooltipPaddingLeft: {
      type: 'number',
      title: '左侧内部间距',
      default: 5,
      values: [0,20]
    },
    tooltipPaddingRight: {
      type: 'number',
      title: '右侧内部间距',
      default: 5,
      values: [0,20]
    },
    tooltipPaddingTop: {
      type: 'number',
      title: '顶部内部间距',
      default: 5,
      values: [0,20]
    },
    tooltipPaddingBottom: {
      type: 'number',
      title: '底部内部间距',
      default: 5,
      values: [0,20]
    },
    tooltipColor: {
      type: 'color',
      title: '内部颜色',
      default: '#eee'
    },
    tooltipBorder: {
      type: 'number',
      title: '边框粗细',
      default: 1,
      values: [0,10]
    },
    tooltipBorderRadius: {
      type: 'number',
      title: '边框圆角弧度',
      default: 0,
      values: [0,100]
    }
    
  };
  