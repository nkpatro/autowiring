#include "stdafx.h"
#include "AutoPacketFactory.h"
#include "AutoPacket.h"
#include "AutoPacketListener.h"

void AutoPacketFactory::AutoPacketResetter::operator()(AutoPacket& packet) const {
  // Notify all listeners that a packet has just returned home:
  m_apl(&AutoPacketListener::OnPacketReturned)(packet);

  // Eliminate references to any shared pointers
  packet.Release();
}

AutoPacketFactory::AutoPacketFactory()
{
}

AutoPacketFactory::~AutoPacketFactory()
{
}

std::shared_ptr<AutoPacket> AutoPacketFactory::NewPacket(void) {
  // Obtain a packet:
  std::shared_ptr<AutoPacket> retVal;
  m_packets(retVal);

  // Fill the packet with satisfaction information:
  retVal->Reset();

  // Done, return
  return retVal;
}

void AutoPacketFactory::AddSubscriber(AutoPacketSubscriber&& rhs) {
  const std::type_info& ti = *rhs.GetSubscriberTypeInfo();

  // Determine whether this subscriber already exists--perhaps, it is formerly disabled
  auto q = m_subMap.find(ti);
  size_t subscriberIndex;
  if(q != m_subMap.end()) {
    AutoPacketSubscriber& entry = m_subscribers[q->second];
    subscriberIndex = q->second;

    if(!entry.empty())
      // Already registered, no need to register a second time
      return;

    // Registered but previously removed, re-initialize
    entry = std::move(rhs);
  }
  else {
    // Register the subscriber and pre-populate the arity:
    subscriberIndex = m_subscribers.size();
    m_subMap[ti] = subscriberIndex;
    m_subscribers.push_back(std::move(rhs));
  }

  // Prime the satisfaction graph for each element:
  for(
    auto ppCur = rhs.GetSubscriberArgs();
    *ppCur;
  ppCur++
    ) {
    // Obtain the decorator type at this position:
    auto r = m_decorations.find(**ppCur);
    if(r == m_decorations.end())
      // Decorator formerly not encountered, introduce it:
      r = m_decorations.insert(
      t_decMap::value_type(
      **ppCur,
      AdjacencyEntry(**ppCur)
      )
      ).first;

    // Now we need to update the adjacency entry with the new subscriber:
    r->second.subscribers.push_back(subscriberIndex);
  }
}