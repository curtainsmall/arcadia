#pragma once

#include<chrono>
#include<format>
#include<memory>
#include<ranges>
#include<string>
#include<vector>

#include"core/base.hpp"
#include"core/event/event.hpp"
#include"core/exception.hpp"

namespace arcadia
{
    struct ARCADIA_API layer_interface: arcadia::noncopyable
    {
    public:
        using self_type = layer_interface;
    public:
        layer_interface(const std::string& name = "layer");
        virtual ~layer_interface() = default;

        [[nodiscard]]
        inline auto get_name() const -> const std::string&
        {
            return _name;
        }

        /// @brief Process event
        /// @param event Event to be processed
        virtual void on_event(arcadia::event_base& event) = 0;

        /// @brief Update layer
        virtual void on_update() = 0;

    private:
        std::string _name{};
    };

    template<class Layer>
    concept layer_like = requires{
        std::derived_from<Layer, arcadia::layer_interface>;
    };

    struct ARCADIA_API layer_stack
    {
    public:
        ARCADIA_EXCEPTION(out_of_range);
        ARCADIA_EXCEPTION(empty_stack);

        using layer_sptr_vector_type = std::vector<std::shared_ptr<layer_interface>>;

        using self_type = layer_stack;
    public:
        static auto instance() -> self_type&;

        template<
            arcadia::layer_like Layer,
            class ...Args
        >
        auto push_layer(Args&& ...args) -> self_type&
        {
            return push_layer(std::make_shared<Layer>(std::forward<Args>(args)...));
        }
        template<
            arcadia::layer_like Layer
        >
        auto push_layer(std::shared_ptr<Layer>&& sptr) -> self_type&
        {
            _layer_sptrs.emplace(_layer_sptrs.begin(), std::move(sptr));
            return *this;
        }
        template<
            arcadia::layer_like Layer,
            class ...Args
        >
        auto push_layer(layer_sptr_vector_type::const_iterator iter, Args&& ...args) -> self_type&
        {
            return push_layer(iter, std::make_shared<Layer>(std::forward<Args>(args)...));
        }
        template<
            arcadia::layer_like Layer
        >
        auto push_layer(layer_sptr_vector_type::const_iterator iter, std::shared_ptr<Layer>&& sptr) -> self_type&
        {
            _layer_sptrs.emplace(
                iter,
                std::move(sptr)
            );
            return *this;
        }
        auto pop_layer() -> self_type&;
        auto pop_layer(layer_sptr_vector_type::const_iterator iter) -> self_type&;
        auto pop_all() -> self_type&;

        template<arcadia::layer_like Layer = arcadia::layer_interface>
        auto at(std::size_t idx) -> Layer&
        {
            if(idx >= size())
            {
                throw out_of_range{ std::format("Index out of range: {}",idx) };
            }

            return static_cast<Layer&>(*_layer_sptrs.at(size() - idx - 1));
        }

        template<arcadia::layer_like Layer = arcadia::layer_interface>
        auto top() -> std::shared_ptr<Layer>
        {
            if(!size())
            {
                throw empty_stack{};
            }
            return std::static_pointer_cast<Layer>(_layer_sptrs.front());
        }

        template<arcadia::layer_like Layer = arcadia::layer_interface>
        auto buttom() -> std::shared_ptr<Layer>
        {
            if(!size())
            {
                throw empty_stack{};
            }
            return std::static_pointer_cast<Layer>(_layer_sptrs.back());
        }


        auto size() -> std::size_t;

        auto begin() -> layer_sptr_vector_type::const_iterator;
        auto end() -> layer_sptr_vector_type::const_iterator;
        auto rbegin() -> layer_sptr_vector_type::const_reverse_iterator;
        auto rend() -> layer_sptr_vector_type::const_reverse_iterator;

    private:
        layer_sptr_vector_type _layer_sptrs{};
    };
}
