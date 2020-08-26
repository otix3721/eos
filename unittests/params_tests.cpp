#include <boost/test/unit_test.hpp>       /* BOOST_AUTO_TEST_SUITE, etc. */

#include <eosio/testing/tester.hpp>       /* tester */
#include <eosio/chain/exceptions.hpp>     /* config_parse_error */

#include <contracts.hpp>                  /* params_test_wasm, params_test_abi */

using namespace eosio;
using namespace eosio::testing;
using mvo = mutable_variant_object;

class params_tester : public tester {
public:
   params_tester() : tester(){
      //set parameters intrinsics are priviledged so we need system account here
      set_code(config::system_account_name, eosio::testing::contracts::params_test_wasm());
      set_abi(config::system_account_name, eosio::testing::contracts::params_test_abi().data());
      produce_block();
   }

   void action(name action_name, mvo mvo){
      push_action( config::system_account_name, action_name, config::system_account_name, mvo );
      produce_block();
   }
};

BOOST_AUTO_TEST_SUITE(params_tests)

BOOST_FIXTURE_TEST_CASE(main_test, params_tester){
   //no throw = success
   action("maintest"_n, mvo());
}

BOOST_FIXTURE_TEST_CASE(throw_test, params_tester){
   BOOST_CHECK_THROW( [&]{action("setthrow1"_n, mvo());}(), chain::config_parse_error);
   BOOST_CHECK_THROW( [&]{action("setthrow2"_n, mvo());}(), fc::out_of_range_exception);
   BOOST_CHECK_THROW( [&]{action("setthrow3"_n, mvo());}(), chain::action_validate_exception);
   BOOST_CHECK_THROW( [&]{action("getthrow1"_n, mvo());}(), chain::config_parse_error);
   BOOST_CHECK_THROW( [&]{action("getthrow2"_n, mvo());}(), fc::out_of_range_exception);
   BOOST_CHECK_THROW( [&]{action("getthrow3"_n, mvo());}(), chain::config_parse_error);
}

BOOST_AUTO_TEST_SUITE_END()
