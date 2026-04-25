#ifndef HIERSIM_MARKET_H
#define HIERSIM_MARKET_H

#include "hiersim/Commodity.h"
#include <string>
#include <map>
#include <vector>
#include <cstdint>
#include <queue>

namespace hiersim {

/**
 * @brief Represents a market where commodities are exchanged.
 * 
 * Markets facilitate the exchange of commodities between producers and consumers.
 * Prices fluctuate based on supply and demand dynamics.
 */
class Market {
public:
    using ID = uint64_t;
    
    /**
     * @brief Order types
     */
    enum class OrderType {
        Buy,
        Sell
    };
    
    /**
     * @brief Market order for a commodity
     */
    struct Order {
        OrderType type;
        Commodity::ID commodityId;
        std::string commodityName;
        int64_t quantity;
        double limitPrice;  // Maximum buy price or minimum sell price
        ID ownerId;         // Organization or Individual placing the order
        int64_t tickPosted;
        
        bool operator<(const Order& other) const {
            // Priority queue ordering: higher prices first for buys, lower for sells
            if (type == OrderType::Buy) {
                return limitPrice < other.limitPrice;
            } else {
                return limitPrice > other.limitPrice;
            }
        }
    };
    
    /**
     * @brief Transaction record
     */
    struct Transaction {
        Commodity::ID commodityId;
        std::string commodityName;
        int64_t quantity;
        double price;
        ID buyerId;
        ID sellerId;
        int64_t tick;
    };
    
    /**
     * @brief Price statistics for a commodity
     */
    struct PriceStats {
        double currentPrice;
        double averagePrice;
        double volatility;
        int64_t volumeTraded;
        int64_t lastUpdateTick;
    };
    
    /**
     * @brief Construct a new Market object
     * @param name Market name
     * @param id Unique identifier
     */
    explicit Market(const std::string& name, ID id = 0);
    ~Market() = default;
    
    // Getters
    ID getId() const { return id_; }
    const std::string& getName() const { return name_; }
    const std::map<std::string, PriceStats>& getPriceStats() const { return priceStats_; }
    const std::vector<Transaction>& getRecentTransactions() const { return recentTransactions_; }
    
    /**
     * @brief Post a buy order to the market
     * @param commodityId ID of the commodity
     * @param commodityName Name of the commodity
     * @param quantity Quantity desired
     * @param limitPrice Maximum price willing to pay
     * @param ownerId ID of the buyer
     * @param currentTick Current simulation tick
     * @return Order ID
     */
    ID postBuyOrder(Commodity::ID commodityId, const std::string& commodityName,
                    int64_t quantity, double limitPrice, ID ownerId, int64_t currentTick);
    
    /**
     * @brief Post a sell order to the market
     * @param commodityId ID of the commodity
     * @param commodityName Name of the commodity
     * @param quantity Quantity available
     * @param limitPrice Minimum price willing to accept
     * @param ownerId ID of the seller
     * @param currentTick Current simulation tick
     * @return Order ID
     */
    ID postSellOrder(Commodity::ID commodityId, const std::string& commodityName,
                     int64_t quantity, double limitPrice, ID ownerId, int64_t currentTick);
    
    /**
     * @brief Cancel an order
     * @param orderId ID of the order to cancel
     * @return true if order was found and cancelled
     */
    bool cancelOrder(ID orderId);
    
    /**
     * @brief Process market clearing (match orders)
     * @param currentTick Current simulation tick
     * @return Number of transactions executed
     */
    size_t clearMarket(int64_t currentTick);
    
    /**
     * @brief Get current market price for a commodity
     * @param commodityId ID of the commodity
     * @return Current price (0 if no trades)
     */
    double getCurrentPrice(Commodity::ID commodityId) const;
    
    /**
     * @brief Get supply/demand imbalance for a commodity
     * @param commodityId ID of the commodity
     * @return Positive for excess demand, negative for excess supply
     */
    double getImbalance(Commodity::ID commodityId) const;
    
    /**
     * @brief Update price statistics after a transaction
     * @param commodityId ID of the commodity
     * @param price Transaction price
     * @param quantity Transaction quantity
     * @param tick Current tick
     */
    void updatePriceStats(Commodity::ID commodityId, const std::string& commodityName,
                          double price, int64_t quantity, int64_t tick);
    
    /**
     * @brief Serialize market data
     * @return Serialized string representation
     */
    std::string serialize() const;
    
    /**
     * @brief Deserialize market from string
     * @param data Serialized data
     * @return Deserialized Market object
     */
    static Market deserialize(const std::string& data);
    
private:
    ID id_;
    std::string name_;
    
    // Order books: commodity name -> orders
    std::map<std::string, std::priority_queue<Order>> buyOrders_;
    std::map<std::string, std::priority_queue<Order>> sellOrders_;
    
    // All orders by ID for cancellation
    std::map<ID, Order> allOrders_;
    ID nextOrderId_;
    
    // Price tracking
    std::map<std::string, PriceStats> priceStats_;
    
    // Recent transactions (circular buffer concept, simplified here)
    std::vector<Transaction> recentTransactions_;
    static constexpr size_t MaxRecentTransactions = 100;
    
    static ID nextId_;
};

} // namespace hiersim

#endif // HIERSIM_MARKET_H