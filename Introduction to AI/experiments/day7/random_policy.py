import numpy as np
import tensorflow as tf
import gym

env = gym.make('CartPole-v0')
#
############################
#    使用随机动作看看      ##
############################
env.reset()
random_episodes = 0
reward_sum = 0
# 在初始化环境之后，我们进行10次随机试验
while random_episodes < 10:
    # 图像渲染
    env.render()
    # 获取observation，reward, done以及info
    # 使用np.random.randint(0,2)产生随机的动作，随后用env.step()执行
    observation, reward, done, _ = env.step(np.random.randint(0,2))
    reward_sum += reward
    # 如果done标记为true，则代表本次试验结束
    # 试验结束表示杆儿的倾斜角度超过了阈值度数或者小车偏离中心过远导致任务失败
    if done:
        random_episodes += 1
        # 试验结束后，我们需要展示本次试验累计获得的奖励reward_sum并重启环境
        print("Reward for this episode was:",reward_sum)
        reward_sum = 0
        env.reset()
env.close()