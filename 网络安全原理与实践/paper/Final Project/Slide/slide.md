---
theme: gaia
_class: lead
paginate: true
header: 2021 Network Security Theory and Practice
backgroundColor: #fff
footer: Group-5
size: 4:3
backgroundImage: url('./img/hero-background.jpg')
marp: true
---

# **HydRand**: Efficient Continuous Distributed Randomness

JiaYing Lu, ShunZheng Tang,
Hao Zheng, Ziyue Pan 

---

<!-- _class: lead -->
# Part 1: Review **HydRand**

---

# Common **Randomness**

![bg 90%](img/common-random.png)

---

# What is **HydRand**

- **Distributed Randomness Protocol**
- Based on **PVSS** Protocol
  - Use Non-Interactive **Zero-Knowledge Proofs**
  - **Malicious Nodes can be Detected**

---

# What is **HydRand**

![bg 80%](img/hydrand-overview.png)

---

# **HydRand** Process

- **Assumption**
  - $n$ nodes, $f$ Byzantine nodes, $n\ge 3f+1$
- **Setup**
  - exchange public key & initialize PVSS shares
- **Execution**
  - propose
  - acknowledge
  - vote → new **random becaon & leader**

---

![bg fit](img/propose1.png)

---

![bg fit](img/propose2.png)

---

![bg fit](img/ack1.png)

---

![bg fit](img/ack2.png)

---

![bg fit](img/vote1.png)

---

![bg fit](img/vote2.png)

---

![bg fit](img/vote3.png)

---

![bg fit](img/vote4.png)

---

![bg fit](img/vote5.png)

---

![bg fit](img/vote6.png)

---

![bg fit](img/vote7.png)

---

![bg fit](img/vote8.png)

---

<!-- _class: lead -->
# Part 2: **Implementation**

---

![bg fit](img/impl-overview.gif)

---

# **Tools**

- Environment 
  - :no_entry_sign: Windows
  - :no_entry_sign: macOS
  - :white_check_mark: Linux
  - :white_check_mark: Docker
- Toolkit
  - Python3
  - **pyzmq** package

---

# **Work**

# **Flow**

Extremely complex, literally.

![bg right:67%](img/flowchart.png)

---

# **Propose**

![bg left](img/impl/flow-propose1.png)

```python
def propose_init():
    # if initial round
    if initialize:
        setup & sync
    
    # increment round
    round += 1
    
    # set phase
    self.phase = 'propose'
```

---
# **Propose**

![bg 72%](img/impl/flow-propose2.png)

---

# **Propose**

```python
def propose():

    propose_init()  # initialize propose phase

    select_leader() # run leader selection algorithm

    if self.role == leader:
        # broadcast proposal
        broadcast_message('propose')

        vote()
    else:
        process_message()
```

---

# **Propose**

![bg 90%](img/impl/flow-propose3.png)

---

# **Propose**

```python
def process_message():
    try:

        >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
            Very Complex Handle Process
        <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

    except (    # catch error
        deserialization_err, 
        invalid_message_structure_err,
        invalid_message
    ) as e:
        sender_is_malicious(sender)
```

---

# **Acknowledge**

![bg 90%](img/impl/flow-ack1.png)

---

# **Acknowledge**

```python
def ack():
    self.phase = 'ack'      # set phase
    try:
        store(message)      # try store message
    except message_err:
        sender_is_malicious(sender) # malicious

    broadcast_message('ack')
    
    if len(acks) < 2f or len(proposes) == 0:
        process_message()
    else:
        vote()
```

---

# **Vote**

```python
def vote():
    self.phase = 'vote'

    if len(acks) > 2f:
        broadcast_message('confirm')
    elif len(confirms) > f + 1:

        >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
            Calculate Random Beacon
        <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

    else:
        broadcast_message('recover')
```

---

<!-- _class: lead -->
# Part 3: **DEMO**

---
<video width="820" height="600" controls>
  <source src="./video/demo.mp4" type="video/mp4">
</video>

---

<!-- _class: lead -->
# Part 4: **Conclusion**

---

# Appealing **Attributes**

- :question: Unpredictability
- :page_facing_up: Public-Verifiability
- :link: Bias-Resistance
- :package: Guaranteed Output Delivery
- :seedling: Liveness
- :chart_with_upwards_trend: Scalability
- :recycle: Energy Efficiency

---

# Appealing **Attributes**

- :question: **Unpredictability**
- :page_facing_up: Public-Verifiability
- :link: Bias-Resistance
- :package: Guaranteed Output Delivery
- :seedling: Liveness
- :chart_with_upwards_trend: Scalability
- :recycle: Energy Efficiency

---

![bg fit](img/eval/unpredictability.png)

---

# Appealing **Attributes**

- :question: Unpredictability :white_check_mark:
- :page_facing_up: **Public-Verifiability**
- :link: Bias-Resistance
- :package: Guaranteed Output Delivery
- :seedling: Liveness
- :chart_with_upwards_trend: Scalability
- :recycle: Energy Efficiency

---

![bg fit](img/eval/PVSS.png)

---

# Appealing **Attributes**

- :question: Unpredictability :white_check_mark:
- :page_facing_up: Public-Verifiability :white_check_mark:
- :link: **Bias-Resistance**
- :package: Guaranteed Output Delivery
- :seedling: Liveness
- :chart_with_upwards_trend: Scalability
- :recycle: Energy Efficiency 

---

_Every benevolent node can be the leader._

![bg 100%](img/eval/decentralized.svg)

---

# Appealing **Attributes**

- :question: Unpredictability :white_check_mark:
- :page_facing_up: Public-Verifiability :white_check_mark:
- :link: Bias-Resistance :white_check_mark:
- :package: **Guaranteed Output Delivery**
- :seedling: **Liveness**
- :chart_with_upwards_trend: Scalability
- :recycle: Energy Efficiency 

---

![bg fit](img/eval/fail.png)

---

![bg fit](img/eval/recover.png)

---
# Appealing **Attributes**

- :question: Unpredictability :white_check_mark:
- :page_facing_up: Public-Verifiability :white_check_mark:
- :link: Bias-Resistance :white_check_mark:
- :package: Guaranteed Output Delivery :white_check_mark:
- :seedling: Liveness :white_check_mark:
- :chart_with_upwards_trend: **Scalability**
- :recycle: Energy Efficiency

---

![bg 100%](img/eval/bandwidth.png)

---

# Appealing **Attributes**

- :question: Unpredictability :white_check_mark:
- :page_facing_up: Public-Verifiability :white_check_mark:
- :link: Bias-Resistance :white_check_mark:
- :package: Guaranteed Output Delivery :white_check_mark:
- :seedling: Liveness :white_check_mark:
- :chart_with_upwards_trend: Scalability :white_check_mark:
- :recycle: **Energy Efficiency**

---

![bg 100%](img/eval/cpu-util.png)

---

# Appealing **Attributes**

- :question: Unpredictability :white_check_mark:
- :page_facing_up: Public-Verifiability :white_check_mark:
- :link: Bias-Resistance :white_check_mark:
- :package: Guaranteed Output Delivery :white_check_mark:
- :seedling: Liveness :white_check_mark:
- :chart_with_upwards_trend: Scalability :white_check_mark:
- :recycle: Energy Efficiency :white_check_mark: 

---

<!-- _class: lead -->
# Perfect? 

---

# Extreme **Complexity**

- $5$ nodes introduce a **minimal** $36$ packets transmission
  - $O(n^2)$ communication complexity
  - Even did not take **failed transmission** into consideraion
- Not an acceptable choice in scenario with a single node

---

![width:150px](img/eval/info.png)

![bg 70%](img/eval/graph.svg)

---

![bg 100%](img/eval/throughput.png)

---

# Application & Improvement

- More **Industrial** Implementation
  - e.g. Rust, F#
- **Multi-theaded** Message Handling
- **Partially Synchronous** Network
  - Verifiable Delay Function (VDF)

---

# Parallelism Optimization

![bg 100%](img/eval/multi-thread.png)

---

# Verifiable Delay Function 

![bg 100%](img/eval/VDF.png)

---

<!-- _class: lead -->
# **Thank You**
