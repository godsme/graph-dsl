////
//// Created by Darwin Yuan on 2020/8/16.
////
//
//#ifndef GRAPH_HANA_TUPLE_TRAIT_H
//#define GRAPH_HANA_TUPLE_TRAIT_H
//
//#include <graph/graph_ns.h>
//#include <boost/hana/tuple.hpp>
//
//GRAPH_DSL_NS_BEGIN
//
//template<typename T, template<typename ...> typename C>
//struct hana_tuple_trait;
//
//template <template<typename ...> typename C, typename ... Ts>
//struct hana_tuple_trait<boost::hana::tuple<Ts...>, C> {
//   using type = C<typename Ts::type...>;
//};
//
//template<typename T, template<typename ...> typename C>
//using hana_tuple_trait_t = typename hana_tuple_trait<std::decay_t<T>, C>::type;
//
//
//GRAPH_DSL_NS_END
//
//#endif //GRAPH_HANA_TUPLE_TRAIT_H
