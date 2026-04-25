#include "hiersim/Market.h"
#include <sstream>
#include <iomanip>
#include <cmath>
#include <algorithm>

namespace hiersim {

// Initialize static members
Market::ID Market::nextId_ = 1;

Market::Market(const std::string& name, ID id)
    : id_(id != 0 ? id : nextId_++)
    , name_(name)
    , nextOrderId_(1)
{
    if (id != 0 && id >= nextId_) {
        nextId_ = id + 1;
    }
}

Market::ID Market::postBuyOrder(Commodity::ID commodityId, const std::string& commodityName,
                                 int64_t quantity, double limitPrice, ID ownerId, int64_t currentTick) {
    Order order;
    order.type = OrderType::Buy;
    order.commodityId = commodityId;
    order.commodityName = commodityName;
    order.quantity = quantity;
    order.limitPrice = limitPrice;
    order.ownerId = ownerId;
    order.tickPosted = currentTick;
    
    ID orderId = nextOrderId_++;
    allOrders_[orderId] = order;
    buyOrders_[commodityName].push(order);
    
    return orderId;
}

Market::ID Market::postSellOrder(Commodity::ID commodityId, const std::string& commodityName,
                                  int64_t quantity, double limitPrice, ID ownerId, int64_t currentTick) {
    Order order;
    order.type = OrderType::Sell;
    order.commodityId = commodityId;
    order.commodityName = commodityName;
    order.quantity = quantity;
    order.limitPrice = limitPrice;
    order.ownerId = ownerId;
    order.tickPosted = currentTick;
    
    ID orderId = nextOrderId_++;
    allOrders_[orderId] = order;
    sellOrders_[commodityName].push(order);
    
    return orderId;
}

bool Market::cancelOrder(ID orderId) {
    auto it = allOrders_.find(orderId);
    if (it == allOrders_.end()) {
        return false;
    }
    
    const Order& order = it->second;
    
    // Remove from appropriate order book
    // Note: This is a simplified implementation - full implementation would need
    // to handle priority_queue removal properly
    allOrders_.erase(it);
    return true;
}

size_t Market::clearMarket(int64_t currentTick) {
    size_t transactionsExecuted = 0;
    
    // Process each commodity market
    for (auto& [commodityName, buyQueue] : buyOrders_) {
        auto sellIt = sellOrders_.find(commodityName);
        if (sellIt == sellOrders_.end()) continue;
        
        auto& sellQueue = sellIt->second;
        
        while (!buyQueue.empty() && !sellQueue.empty()) {
            Order bestBuy = buyQueue.top();
            Order bestSell = sellQueue.top();
            
            // Check if prices match (buy price >= sell price)
            if (bestBuy.limitPrice < bestSell.limitPrice) {
                break;  // No more matches possible
            }
            
            // Execute transaction at midpoint price (simplified)
            double execPrice = (bestBuy.limitPrice + bestSell.limitPrice) / 2.0;
            int64_t execQty = std::min(bestBuy.quantity, bestSell.quantity);
            
            // Record transaction
            Transaction tx;
            tx.commodityId = bestBuy.commodityId;
            tx.commodityName = commodityName;
            tx.quantity = execQty;
            tx.price = execPrice;
            tx.buyerId = bestBuy.ownerId;
            tx.sellerId = bestSell.ownerId;
            tx.tick = currentTick;
            
            recentTransactions_.push_back(tx);
            if (recentTransactions_.size() > MaxRecentTransactions) {
                recentTransactions_.erase(recentTransactions_.begin());
            }
            
            // Update price statistics
            updatePriceStats(bestBuy.commodityId, commodityName, execPrice, execQty, currentTick);
            
            // Update or remove orders
            buyQueue.pop();
            sellQueue.pop();
            
            if (bestBuy.quantity > execQty) {
                bestBuy.quantity -= execQty;
                buyQueue.push(bestBuy);
            }
            if (bestSell.quantity > execQty) {
                bestSell.quantity -= execQty;
                sellQueue.push(bestSell);
            }
            
            // Remove from allOrders
            // (In a full implementation, we'd track which order IDs were fully/partially filled)
            
            transactionsExecuted++;
        }
    }
    
    return transactionsExecuted;
}

double Market::getCurrentPrice(Commodity::ID commodityId) const {
    // Find most recent transaction for this commodity
    for (auto it = recentTransactions_.rbegin(); it != recentTransactions_.rend(); ++it) {
        if (it->commodityId == commodityId) {
            return it->price;
        }
    }
    return 0.0;
}

double Market::getImbalance(Commodity::ID commodityId) const {
    // Calculate total buy vs sell quantity
    int64_t totalBuy = 0;
    int64_t totalSell = 0;
    
    // This is simplified - full implementation would iterate through order books
    // For now, return 0 as placeholder
    return static_cast<double>(totalBuy - totalSell);
}

void Market::updatePriceStats(Commodity::ID commodityId, const std::string& commodityName,
                               double price, int64_t quantity, int64_t tick) {
    auto& stats = priceStats_[commodityName];
    
    // Update current price
    stats.currentPrice = price;
    
    // Update volume
    stats.volumeTraded += quantity;
    
    // Update average price (simple moving average)
    if (stats.lastUpdateTick == 0) {
        stats.averagePrice = price;
    } else {
        // Weighted average based on volume
        double totalVolume = stats.volumeTraded;
        stats.averagePrice = ((stats.averagePrice * (totalVolume - quantity)) + (price * quantity)) / totalVolume;
    }
    
    // Update volatility (simplified - standard deviation proxy)
    double diff = std::abs(price - stats.averagePrice);
    stats.volatility = 0.9 * stats.volatility + 0.1 * diff;
    
    stats.lastUpdateTick = tick;
}

std::string Market::serialize() const {
    std::ostringstream oss;
    oss << id_ << "|" << name_ << "|" << nextOrderId_;
    // Full serialization would include order books and transactions
    return oss.str();
}

Market Market::deserialize(const std::string& data) {
    Market market("placeholder", 0);
    std::istringstream iss(data);
    
    char delim;
    iss >> market.id_ >> delim;
    std::getline(iss, market.name_, delim);
    iss >> market.nextOrderId_;
    
    if (market.id_ >= nextId_) {
        nextId_ = market.id_ + 1;
    }
    
    return market;
}

} // namespace hiersim