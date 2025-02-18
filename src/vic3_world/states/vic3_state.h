#ifndef SRC_VIC3WORLD_STATES_VIC3STATE_H
#define SRC_VIC3WORLD_STATES_VIC3STATE_H



#include <set>



namespace vic3
{

struct StateOptions
{
   std::optional<int> owner_number;
   std::optional<std::string> owner_tag;
   bool incorporated = false;
   std::set<int> provinces;
   int population = 0;
   int employed_population = 0;
};


class State
{
  public:
   State() = default;
   explicit State(StateOptions state_options):
       owner_number_(state_options.owner_number),
       owner_tag_(std::move(state_options.owner_tag)),
       incorporated_(state_options.incorporated),
       provinces_(std::move(state_options.provinces)),
       population_(state_options.population),
       employed_population_(state_options.employed_population)
   {
   }

   [[nodiscard]] const std::optional<int>& GetOwnerNumber() const { return owner_number_; }
   [[nodiscard]] const std::optional<std::string>& GetOwnerTag() const { return owner_tag_; }
   [[nodiscard]] bool IsIncorporated() const { return incorporated_; }
   [[nodiscard]] const std::set<int>& GetProvinces() const { return provinces_; }
   [[nodiscard]] int GetPopulation() const { return population_; }
   [[nodiscard]] int GetEmployedPopulation() const { return employed_population_; }

   void SetOwnerTag(std::string_view owner_tag) { owner_tag_ = owner_tag; }

   bool operator==(const State&) const = default;

  private:
   std::optional<int> owner_number_;
   std::optional<std::string> owner_tag_;
   bool incorporated_ = false;
   std::set<int> provinces_;
   int population_ = 0;
   int employed_population_ = 0;
};

}  // namespace vic3



#endif  // SRC_VIC3WORLD_STATES_VIC3STATE_H