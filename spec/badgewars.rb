require File.expand_path('../../ext/badgewars', __FILE__)

class << BadgeWars::Op
  def [](op, lhs = 0, rhs = 0)
    BadgeWars::Op.new(op, lhs, rhs)
  end
end