# ProducerConsumer-CandyFactory
Multithreaded Application <br />
1. Producer - factories that create candies to be consumed
2. Consumer - kids that consume candies created by factories<br />

Threads:
<ul>
  <li>FactoryThread: produces candy and calls method to insert it into the buffer<br />
    <img src="images/factoryThread.png" alt="FactoryThread" height="450">
</li>
  <li>KidThread: calls method that checks if candy is available and removes it from the buffer<br />
      <img src="images/kidThread.png" alt="KidThread" height="300">
</li>
  </ul>
  
  Semaphores:
  <ul>
  <li>Empty: ensures the buffer has space for thread to insert a new candy</li>
  <li>Full: ensures the buffer has candy for thread to consume<br />
      <img src="images/semaphores.png" alt="semaphores" height="150">

  </li>
  </ul>
  
Mutex: ensures only one thread accesses the shared buffer to prevent race conditions <br />
    <img src="images/mutex.png" alt="mutex" height="300">

<br />
<b>Sample output:</b> <br />
    <img src="images/output.png" alt="output" height="450">
