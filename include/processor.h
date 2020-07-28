#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization(); 


 private:
  float m_prev_alltime{0.0f};
  float m_prev_nonidletime{0.0f};
};

#endif