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
        using delta_time_type = std::chrono::milliseconds;

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
        virtual void on_update(delta_time_type delta_time) = 0;

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

        using layer_uptr_vector_type = std::vector<std::unique_ptr<layer_interface>>;

        using self_type = layer_stack;
    public:
        static auto instance() -> self_type&;

        template<
            arcadia::layer_like Layer,
            class ...Args
        >
        auto push_layer(Args&& ...args) -> self_type&
        {
            return push_layer(std::make_unique<Layer>(std::forward<Args>(args)...));
        }
        template<
            arcadia::layer_like Layer
        >
        auto push_layer(std::unique_ptr<Layer>&& uptr) -> self_type&
        {
            _layer_uptrs.emplace_back(std::move(uptr));
            return *this;
        }
        template<
            arcadia::layer_like Layer,
            class ...Args
        >
        auto push_layer_at(std::size_t idx, Args&& ...args) -> self_type&
        {
            return push_layer_at(idx, std::make_unique<Layer>(std::forward<Args>(args)...));
        }
        template<
            arcadia::layer_like Layer
        >
        auto push_layer_at(std::size_t idx, std::unique_ptr<Layer>&& uptr) -> self_type&
        {
            if(idx >= _layer_uptrs.size())
            {
                throw out_of_range{ std::format("Index out of range: {}",idx) };
            }

            _layer_uptrs.emplace(
                _layer_uptrs.begin() + idx,
                std::move(uptr)
            );
            return *this;
        }
        auto pop_layer() -> self_type&;
        auto pop_layer_at(std::size_t idx) -> self_type&;
        auto pop_all() -> self_type&;

        template<arcadia::layer_like Layer = arcadia::layer_interface>
        auto at(std::size_t idx) -> Layer&
        {
            if(idx >= size())
            {
                throw out_of_range{ std::format("Index out of range: {}",idx) };
            }

            return static_cast<Layer&>(*_layer_uptrs.at(size() - idx - 1));
        }

        template<arcadia::layer_like Layer = arcadia::layer_interface>
        auto top() -> Layer&
        {
            if(!size())
            {
                throw empty_stack{};
            }
            return static_cast<Layer&>(*_layer_uptrs.back());
        }

        template<arcadia::layer_like Layer = arcadia::layer_interface>
        auto buttom() -> Layer&
        {
            if(!size())
            {
                throw empty_stack{};
            }
            return static_cast<Layer&>(*_layer_uptrs.front());
        }


        auto size() -> std::size_t;

        auto begin() -> layer_uptr_vector_type::const_iterator;
        auto end() -> layer_uptr_vector_type::const_iterator;
        auto rbegin() -> layer_uptr_vector_type::const_reverse_iterator;
        auto rend() -> layer_uptr_vector_type::const_reverse_iterator;

    private:
        layer_uptr_vector_type _layer_uptrs{};
    };
}
