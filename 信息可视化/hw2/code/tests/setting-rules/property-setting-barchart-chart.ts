// 这里记录的是图表各种默认配置，当用户在测试页面选择不同的选项时，也就是是选择了这里的某组配置参数。
export default {
    chartBackground: {
      type: 'color',
      title: '背景色',
      default: '#eee'
    },
    chartPaddingLeft: {
      type: 'number',
      title: '左侧内部间距',
      default: 80,
      values: [0,200]
    },
    chartPaddingRight: {
      type: 'number',
      title: '右侧内部间距',
      default: 80,
      values: [0,200]
    },
    chartPaddingTop: {
      type: 'number',
      title: '顶部内部间距',
      default: 80,
      values: [0,200]
    },
    chartPaddingBottom: {
      type: 'number',
      title: '底部内部间距',
      default: 80,
      values: [0,200]
    },
    scrollingWidth: {
      type: 'number',
      title: '滚动宽度',
      default: 720,
      values: [0,1300]
    },
    scrollingHeight: {
      type: 'number',
      title: '滚动高度',
      default: 720,
      values: [0,1000]
    },
    labelIsShow: {
      type: 'boolean',
      title: '是否显示数据标签',
      default: true
    },
    labelPosition: {
      type: 'enum',
      title: '数据标签位置',
      default: 'top',
      values: ['top','bottom','innner']
    },
    scrollingIsShow: {
      type: 'boolean',
      title: '是否允许滚动',
      default: false
    },
    sortType: {
      type: 'enum',
      title: '排序方式',
      default: 'null',
      values: ['null', 'ascending','descending']
    }
    
  };
  