// 这里记录的是图表各种默认配置，当用户在测试页面选择不同的选项时，也就是是选择了这里的某组配置参数。
export default {
    yAxisIsShow: {
      type: 'boolean',
      title: '是否显示y轴',
      default: true
    },
    yAxisTickIsShow: {
      type: 'boolean',
      title: '是否显示y轴刻度',
      default: true
    },
    yAxisLabelIsShow: {
      type: 'boolean',
      title: '是否显示y轴标签',
      default: true
    },
    yAxisPosition: {
      type: 'enum',
      title: 'y轴位置',
      default: 'left',
      values: ['left','right']
    },
    yAxisNice: {
      type: 'boolean',
      title: '是否将y轴定义域范围扩展至理想形式',
      default: true
    },
    yAxisGridIsShow: {
      type: 'boolean',
      title: '是否显示y轴网格线',
      default: true
    }
  };
  