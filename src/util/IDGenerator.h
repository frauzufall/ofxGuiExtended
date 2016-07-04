#pragma once

class IDGenerator {
   public:
      static IDGenerator&     getInstance() {
          static IDGenerator instance;
          return instance;
      }
      int next () { return _id++; }
   private:
      IDGenerator () : _id(0) {}
      int _id;
};
