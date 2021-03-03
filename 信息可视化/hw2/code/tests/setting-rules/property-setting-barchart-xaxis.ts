// 这里记录的是图表各种默认配置，当用户在测试页面选择不同的选项时，也就是是选择了这里的某组配置参数。
export default {
    xAxisIsShow: {
      type: 'boolean',
      title: '是否显示x轴',
      default: true
    },
    xAxisTickIsShow: {
      type: 'boolean',
      title: '是否显示x轴刻度',
      default: true
    },
    xAxisLabelIsShow: {
      type: 'boolean',
      title: '是否显示x轴标签',
      default: true
    },
    xAxisPosition: {
      type: 'enum',
      title: 'x轴位置',
      default: 'bottom',
      values: ['top','bottom']
    },
    xAxisGridIsShow: {
      type: 'boolean',
      title: '是否显示x轴网格线',
      default: false
    }
  };
  