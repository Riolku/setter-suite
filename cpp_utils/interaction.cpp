void preInteractorError();

template <typename Parent> class InteractorReader : public Parent {
protected:
  void preError() override { preInteractorError(); }

public:
  using Parent::Parent;

  // close stdout before reading EOF.
  ~InteractorReader() { fclose(stdout); }
};
