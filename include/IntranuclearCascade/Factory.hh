#ifndef FACTORY_HH
#define FACTORY_HH

#include <memory>
#include <unordered_map>

namespace IntranuclearCascade {

/// The InteractionFactory creates a method of generating an interaction model for the 
/// intranuclear cascade at runtime by a string in the run card. This allows the user to 
/// test how different interaction models may effect results without having to recompile any
/// code.
template<class Base, typename... Args>
class Factory {
    public:
        static Factory* Instance() {
            static Factory instance;
            return &instance;
        }

        template<typename T>
        void reg(const std::string &name) {
            data()[name].reset(new Creator<T>); 
        }

        static std::shared_ptr<Base> Create(const std::string &name, Args&&... args) {
            return data().at(name) -> create(std::forward<Args>(args)...); 
        }

    private:
        struct ICreator {
            virtual std::shared_ptr<Base> create(Args&&...) = 0; 
        };

        template<typename T>
        struct Creator : public ICreator {
             std::shared_ptr<Base> create(Args&&...args) override {
                return std::make_shared(T(std::forward<Args>(args)...)); 
            } 
        };

        static auto &data() {
            static std::unordered_map<std::string, std::unique_ptr<ICreator>> s;
            return s;
        }
};

template<class Base, typename T, typename... Args>
class Register {
    public:
        Register(const std::string &name) {
            Factory<Base, Args...>::Instance() -> template reg<T>(name);
        }
};

}

#endif
