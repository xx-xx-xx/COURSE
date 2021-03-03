import Vue from 'vue';
import VueRouter, { RouteConfig } from 'vue-router';

Vue.use(VueRouter);

const routes: Array<RouteConfig> = [
  {
    path: '/',
    name: 'bar-chart',
    component: () => import('../views/bar-chart/bar-chart.vue'),
    meta: {
      title: 'bar-chart',
    },
  },
  {
    path: '/tree',
    name: 'tree',
    component: () => import('../views/tree/tree.vue'),
    meta: {
      title: 'tree',
    },
  },
  {
    path: '/tree-map',
    name: 'tree-map',
    component: () => import('../views/tree-map/tree-map.vue'),
    meta: {
      title: 'tree-map',
    },
  },
  {
    path: '/sunburst',
    name: 'sunburst',
    component: () => import('../views/sunburst/sunburst.vue'),
    meta: {
      title: 'sunburst',
    },
  }
];

const router = new VueRouter({
  routes,
});

export default router;
