require File.expand_path('../spec_helper', __FILE__)

describe 'BadgeWars' do
  before :each do
    @world = BadgeWars::World.new
  end
  
  it "has a default world state of 0" do
    BadgeWars.core_size.times do |n|
      @world.peek(n).should be_zero
      @world[n].should be_zero
    end
  end
  
  it "can peek and poke the core" do
    @world.peek(0).should be_zero
    @world[0].should be_zero
    
    @world.poke(0, 42)
    @world.peek(0).should == 42
    
    @world[0] = 420
    @world[0].should == 420
  end
end