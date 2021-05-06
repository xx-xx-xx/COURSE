# reading notes

HydRand: Efficient Continuous Distributed Randomness

# I. INTRODUCTION

## 拜占庭将军问题

简单来说，在1982年，拜占庭将军问题被认为是一个逻辑上的困境，它说明了一群拜占庭将军在试图就下一步行动达成统一意见时可能存在的沟通问题。

该困境假设每个将军都有自己的军队，每支军队都位于他们打算攻击的城市周围的不同位置。 这些将军需要就攻击或撤退达成一致。只要所有将军达成共识，即协调后决定共同执行，无论是攻击还是撤退都无关紧要。

因此，我们可以考虑以下条件：

- 每个将军必须作出决定：攻击或撤退（是或否）;
- 一旦做出决定后无法改变;
- 所有将军都必须就统一决定达成一致，并**同步**执行。

上述沟通问题涉及这样一个事实，即一个将军只能通过情报员发送的信息与另一个将军进行通信。因此，拜占庭将军问题的核心挑战是信息可能以某种方式被延迟传达，破坏或丢失。

此外，即使消息成功传递，一个或多个将军也可以选择（出于任何原因）恶意行动并发送欺诈性消息以混淆其他将军，导致行动失败。

如果我们将这个困境对应到区块链上，则每个将军代表一个网络节点，而节点需要就系统的当前状态达成共识。换句话来说，分布式网络中的大多数参与者必须同意并执行相同的操作以避免失败。

因此，在这些类型的分布式系统中达成共识的唯一方法是至少拥有三分之二以上的可靠和诚实的网络节点。这意味着如果网络中的大多数节点决定采取恶意行为，系统就很容易受到故障和攻击（例如51％攻击）。

## 拜占庭容错（BFT）

简单来说，拜占庭容错（BFT）是能够抵抗拜占庭将军问题所导致的一系列失败的系统属性。 这意味着即使某些节点出现故障或恶意行为，拜占庭容错系统也能够继续运行。

拜占庭将军问题有多种可能的解决方案，因此，有多种方法可以构建拜占庭容错系统。同样地，区块链有各种不同的方法来实现拜占庭容错，这就是我们说的**共识算法**。

## 区块链共识算法 consensus algorithm

我们可以将共识算法定义为区块链网络达成共识的机制。最常见的例子是工作量证明（PoW）和权益证明（PoS）。这里我们以比特币为例。

比特币协议规定了系统的主要规则，工作量证明共识算法说明了如何遵循这些规则以达成共识（例如，在交易的验证和验证期间）。

虽然工作量证明的概念比数字货币更早，但中本聪在原版本上进行了修改，并开发了一种改进后的工作量证明算法，可以将比特币的产生作为一种拜占庭容错系统。

请注意，该工作量证明算法不能完全抵抗拜占庭故障，但由于高成本的挖矿过程和底层加密技术，工作量证明已被证明是区块链网络中的一种最安全可靠的方法。从这个意义上说，由中本聪设计的工作量证明共识算法被许多人认为是拜占庭容错最高明的解决方案之一。

![image-20210305214635790](C:\Users\saisai\AppData\Roaming\Typora\typora-user-images\image-20210305214635790.png)

http://www.lianmenhu.com/blockchain-4817-4



![image](https://user-images.githubusercontent.com/8554143/36976961-6242fa0a-20ba-11e8-94c0-ac482b0305e6.png)

PoW 和 PoS 是 2 种不同的对记账权利的分配方式。

### PoW 工作量证明

你能够获得的币的数量，取决于你挖矿贡献的有效工作，也就是说，你用于挖矿的矿机的性能越好，分给你的收益就会越多，这就是根据你的工作证明来执行币的分配方式。

**比特币**采用的共识算法就是 PoW，专业一点说，矿工们在挖一个新的区块时，必须对SHA-256密码散列函数进行运算，区块中的随机散列值以一个或多个0开始。随着0数目的上升，找到这个解所需要的工作量将呈指数增长，矿工通过反复尝试找到这个解。

通俗的说，PoW 的意思就是**社会主义，按劳分配，多劳多得。**

### PoS 股权证明

由于 BTC 的 PoW 机制决定了谁的算力强谁就能获得更多收益，拥有更大的记账权。所以类似比特币这样的 PoW 币种挖矿带来了巨大的电力能源消耗，为了解决这种情况，所以有了 PoS。

PoS 试图解决 PoW 机制中大量资源被浪费的情况。这种机制通过计算你持有占总币数的百分比以及占有币数的时间来决定记账权。

在现实世界中 PoS 很普遍，最为熟知的例子就是**股票**。股票是用来记录股权的证明，股票持有量多的，拥有更高更多的投票权和收益权。

通俗的说，PoS 就是**资本主义，按钱分配，钱生钱。**

https://www.cnblogs.com/coco1s/p/8513376.html

### Proof-of-Delay

### 授权拜占庭容错算法（dBFT，Delegated Byzantine Fault Tolerance）

授权拜占庭容错算法，简称 dBFT，是一种支持通过代理投票实现大规模参与共识的拜占庭容错共识算法。在 Neo 中，令牌持有者可以通过投票选取其支持的 bookkeeper。之后，选定的 bookkeeper 组采用 BFT 算法达成共识，并生成新区块。Neo 网络中的投票是实时的，而非因人而异的。

dBFT 可为具有个共识节点的共识系统提供容错。这种容错也涵盖了安全性和可用性、不受将军和拜占庭错误影响，并且适合任何网络环境。dBFT 具有很好的最终性（finality），这意味着一旦最终确认，区块将不可分叉，交易将不可再撤销或是回滚。

Neo 的 dBFT 机制生成一个区块需 15 到 20 秒钟。交易吞吐量测定约为 1000 TPS。这对于公共区块链而言，这是很好的性能。通过一定优化，dBFT 具有达到一万 TPSS 的潜力，这样就可支持大规模的商业应用。

dBFT 中加入了数字身份技术，这意味着 bookkeeper 可以是真实的个人，也可以是某些机构。因此，dBFT 根据存在于其本身之中的司法判决，可以冻结、撤销、继承、检索和拥有代币兑换权。它有利于实现合规金融资产在 Neo 网络中的注册。Neo 网络从设计上，就是在必要时为此提供支持。

## Random Beacon Protocol

- availability/liveness
- unpredictability
- bias-resistance
- public-verifiability
- (uniform agreement)

### Cryptographic Beacons/Randomness Beacons 密码信标/随机信标

http://www.copenhagen-interpretation.com/home/cryptography/cryptographic-beacons

随机信标是一项提供随机性公开资料来源的服务。信标会以正常的频率不断发出新的随机数据 (信标记录)。如果每个人都同意没有办法预测信标的下一个输出，它可以作为一个公平随机值的提供者。

信标发出的随机值可以用于任何公共彩票，但密码学家还提出了许多其他有趣的应用，这些应用需要在生成之前无法预测的随机值，但在生成之后会公开——这些应用包括合同签署、投票协议和零知识证明

信标必须是:

- 不可预知的: 当前位置敌方在信标记录发布之前预测任何信息的能力是可以忽略不计的
- 无偏见的: 信标记录在统计学上接近于均匀随机字符串
- 普遍可取样的: 在信标记录公布后，任何一方都可以不受限制地使用它
- 全世界都可以证实: 信标记录取样的随机性来源，在信标记录发表之前可以被任何一方核实为未知

信标是不可预测的这一事实也意味着它不能被操纵(因为这将使得有可能以不容忽视的准确度预测其结果)

![img](https://upyun-assets.ethfans.org/uploads/photo/image/4c83fd351f614e8f991b8a94860a2b72.png)

## 分布式密钥生成（DKG）

https://ethfans.org/posts/randomness-threshold-signatures

![img](https://upyun-assets.ethfans.org/uploads/photo/image/563daaf2724a4139a5e0630852dfbb7f.png)

# II. SYSTEM MODEL 系统模型

一组节点，也就是参与者的数量是n=3f+1，f是会出现拜占庭故障的最多节点数

如果一个node没有参与任何不正确行为，它就是correct的，否则就是faulty

faulty nodes=Byzatine=malicious

nodes的集合是p={1, 2, ..., n}

每个node $i\in P$ 有一个公钥/私钥对$<sk_i,pk_i>$

公钥是公开的

# III. PROTOCOL OVERVIEW

# IV. PROTOCOL DETAILS

# V. PROTOCOL PROPERTIES

## 协议

- rounds
  - propose
  - acknowledge
  - voting
- leader
  - reveal 秘密值 and attach next commitment 
  - unreveal and unattach
    - 之前的秘密值由f+1个其他节点重建，其中至少包括一个correct的。PVSS保证了重建的得到的随机信标值恒等于reveal时候的，因此保证了bias-resistance。

因为协议在每一轮输出一个随机信标值，独立于(潜在敌对的)领导者和其他故障节点的动作，保证了guaranteed output delivery(G.O.D.)

PVSS方案的属性保证了Public-verifiability 

# VI. comparison

![](https://gitee.com/xx-xx-xx/images/raw/master/img/20210307130130.png)

PVSS

- Ouroboros
- Scrape
- RandShare
- RandHound
- RandHerd

## Communication Model

We classify the communication model of the analyzed
protocols into three categories, namely synchronous, semi-
synchronous and asynchronous protocols. 

- Algorand and Ouroboros Praos
  - semi-synchronous
- 

## Availability/Liveness

> Any single participant or colluding adversary should not
> be able to prevent progress

In regard to liveness, we distinguish between three different
protocol types:
1) protocols which achieve liveness unconditionally (in the
respective system model)
2) protocols which have a (configurable) but non-zero
probability of a liveness failure
3) protocols which do not provide liveness (in the respec-
tive system model)

We mark protocols of the first type with a ! symbol in
our comparison table.

For protocols of the second type,  we give a typical failure probability as described by the respective authors. 

## Unpredictability 

> Correct, as well as adversarial nodes, should not be able to predict (precompute) future random beacon values.

- Cachin et al., Ouroboros, Dfinity, RandShare, RandHound, RandHerd, Scrapehais
  - all future random beacon values are fully unpredictable
    for all participants
  - 每轮的random beacon取决于参与者数量
- Proof-of-Work, Algorand, Caucus, Ouroboros Praos
  - random beacon 取决于一个node的secret value

- Proof-of-Delay
  - full unpredictability or unpredictability 

- **HydRand**
  - unpredictability with exponentially increasing probability for
    at most f rounds
  - full unpredictability after f + 1 rounds

## Bias-Resistance

> Any single participant or colluding adversary should not be able to influence future random beacon values to their advantage.

- PoW不能保证bias-resistance 
  - When directly relying on the block hashes of a PoW blockchain as a source of  randomness, bias-resistance can generally not be ensured.
- PoD能保证但是计算量很大很慢
  - addresses this problem by employing a delay function on top of the PoW block hash. 
  - While the adversary can still withhold its value
    and influence the protocol’s output, they can only do so
    blindly without knowing the effects at the time of the decision,
    which ensures bias-resistance.
  - full verification of a random beacon value is slow, as it requires the same sequential recomputing
- Proof-of-Work and Proof-of-Delay inherently require a substantial amount of  computational resources to ensure security. 
- Algorand, Ouroboros Praos
  - 不能保证bias-resistance
- Cachin et al., Dfinity, Ouroboros, Proof-of-Delay, RandShare,
  RandHerd, Scrape, **HydRand**
  - strong notion of bias-resistance
- The properties of the underlying PVSS scheme ensure that the random beacon value obtained by reconstruction is always equal to the value that is obtained when a leader reveals his secret – this establishes bias-
  resistance. 
- 

## Energy Efficiency

### Communication Complexity

![](https://gitee.com/xx-xx-xx/images/raw/master/img/20210308145515.png)

- O(n)
  - In the simplest scenario, where a Proof-of-Work blockchain forms the basis for the random beacon, a successful miner only has to perform one broadcast, leading to a complexity of O(n). 
  - This also applies for the Proof-of-Delay approach. 
  - Caucus also provides a low communication complexity of O(n) by leveraging the properties of the under-lying Smart Contract platform.

- O(cn)
  - For the Algorand and Ouroboros Praos protocols, an anal-
    ysis of the communication complexity is not provided in the
    respective publications. 
  - We infer that Ouroboros Praos has a communication complexity in $O(n)$, because the protocol only provides guarantees for eventual consensus and is based upon many of the design principles of Proof-of-Work blockchains,
  -  whereas protocols like Algorand, which provide consensus finality, generally operate at a higher per round communication cost.
  - To optimize the amount of data transmitted, the Algorand
    and Dfinity protocols perform certain communication-heavy operations only within a single subset of nodes.



RandHound and RandHerd employ **sharding** to split nodes into multiple smaller  groups, where some operations are performed independently within all groups, and  the results from individual groups are then combined in a final step. 

$O(n^2)$

As the authors outline, for small values of n, Dfinity’s random beacon protocol may also be executed by all nodes, i.e. without selecting a committee as a subset of all nodes. In this case n nodes broadcast a signature share to all other nodes, leading to a complexity of O(n2)

$O(n^3)$

- The approaches employed by Ouroboros, RandShare and
  Scrape are similar, where each node in the protocol employs
  a PVSS scheme to commit to a secret value. This involves the
  distribution of the PVSS shares, i.e. each node has to broadcast
  a message of size O(n) to all other nodes. The resulting
  communication complexity of O(n3) is a major drawback of
  these approaches, however in this context (P)VSS can also
  help to achieve guaranteed output delivery
- HydRand also uses PVSS as an underlying primitive, but improves efficiency by a factor of O(n) because only a single node has to perform the distribution of PVSS shares per round. The communication com-
  plexity is reduced by shifting the transmission of messages of
  size n to the leader and employing cryptographically signed
  conformation/recovery certificates to converge on a history of
  datasets. 

### Computation and Verification Complexity

(i) computation complexity describes the amount of operations an
active protocol participant has to perform during one round of the protocol

verification complexity, referring to the amount of computation an external (passive) observer of the protocol has to perform in order to verify the correctness of one random beacon value.



## Public-Verifiability

> Third parties not directly participating in the protocol should also be able to verify generated values. As soon as a new random beacon value becomes available, all parties can verify the correctness of the new value using public information only.

## uniform agreement

> all correct nodes agree on a common sequence of random beacon values

uniform agreement is satisfied for the random beacon values
in HydRand

(i) within f + 1 rounds a correct node is selected as leader 

(ii) all correct nodes agree on a common set of potential leaders

## G.O.D guaranteed output delivery

> i.e.that is, the inability for an adversary to prevent correct participants of the protocol from obtaining an output

Additionally, guaranteed output delivery follows because the protocol outputs a random beacon value at each round, independent of the actions of the (potentially adversarial) leader and other faulty nodes.

## avoidance of an initial trusted setup

> e.g. a trusted dealer

* Our protocol is described in a self contained manner and neither
  relies on a trusted dealer nor on a distributed key generation
  (DKG) protocol.

  

 non-interactive zero-knowledge (NIZK) 非交互式零知识证明

https://en.wikipedia.org/wiki/Non-interactive_zero-knowledge_proof



https://zhuanlan.zhihu.com/p/39271872

![img](https://pic2.zhimg.com/80/v2-49d924ba2348962aa7ede636c622d2ed_720w.jpg)

Merkle树就是，我可以准确告诉你，你不是真值。但是真值是什么，无可奉告。



PoW不能保证bias-resistance

PoD能保证但是计算量很大很慢

# VII. DISCUSSION

# VIII. CONCLUSION