class RandomSetBit : public SetWithBIT {
public:
  using SetWithBIT::SetWithBIT;

  int random() const { return (*this)[randint(1, size())]; }
  int pop_random() {
    int x = random();
    remove(x);
    return x;
  }
};
