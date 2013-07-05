=============================
Lovelace Programming Language
=============================


Specification
=============


.. code::

    -module(std).

    -type boolean() ::
          | false
          | true.

    -type comparison() ::
          | less
          | equal
          | greater.

    -type option(T) ::
          | none
          | some {T}.

    %% end module.


    -behaviour_info(comparable(T)).

    -spec compare(T,T) -> std:comparison().

    %% end behaviour_info.


    -behaviour_info(map_container(T, K, E)).

    -spec is_empty(T) -> std:boolean().

    -spec lookup(K, T) -> E.

    %% end behaviour_info.


    -behaviour_info(key_comparable(K, E))

    -spec compare(K, E) -> std:comparison().

    %% end behaviour_info

    -module(comparable_key_comparable).

    -behaviour key_comparable(E,E) when
         comparable(E).

    compare(X, Y) ->
        comparable:compare(X, Y).
    %% end module


    -module(bst).

    -type bst(E)
         when key_comparable(E) ::
         | leaf,
         | tree {E, tree(E), tree(E)}.

    -behaviour map_container(bst(E), K, E) when
        key_comparable(K, E).

    -spec empty() -> bst(E)
        when key_comparable(E).

    empty() ->
        nil.

    is_empty(leaf) ->
        true;
    is_empty(_) ->
        false.

    lookup(_, leaf) ->
        none;
    lookup(Key, tree {Elem, Smaller, Bigger}) ->
        case key_comparable:compare(Key, Elem) of
            equal -> some(Elem);
            less -> lookup(Key, Smaller);
            greater -> lookup(Key, Bigger)
        end.
    %% end module


    -behaviour_info(set(T,E)).

    -spec is_element(E, T) -> std:boolean().

    %% end behaviour_info


    -module(map_container_set).

    -behaviour set(T, E) when
        map_container(T, E, E).

    is_element(Elem, Set) ->
        case map_container:lookup(Elem, Set) of
            none -> false;
            some {_} -> true
        end.
    %% end module

    
    -behaviour_info(map(T,K,V)).

    -spec is_defined(K, T) -> std:boolean().

    %% end behaviour_info


    -module(map_container_map).

    -type map_entry(K, V)
        when comparable(K) ::
          #{ key = K,
             value = V}.

    -behaviour key_comparable(K, map_entry(K,V)).

    -behaviour map(T,K,V) when
        map_container(T, K, map_entry(K,V)).

    compare(K1, #{key=K2}) ->
        comparable:compare(K1, K2).

    is_defined(Key, Tree) ->
        case map_container:lookup(Key, Tree) of
            none -> false;
            some {_} -> true
        end.
    %% endmodule
