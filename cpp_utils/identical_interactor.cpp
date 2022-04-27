class IdenticalInteractorReader
    : public InteractorReader<IdenticalCheckerReader> {

protected:
  FILE *getHelperStream() override { return stderr; }

public:
  using InteractorReader<IdenticalCheckerReader>::InteractorReader;
};
